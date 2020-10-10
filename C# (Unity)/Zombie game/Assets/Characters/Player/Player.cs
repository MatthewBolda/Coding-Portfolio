using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    /////////////////////////////////
    ///Movement + Camera Varaibles///
    /////////////////////////////////
    public CharacterController controller;
    public Transform groundCheck;

    public float speed = 12f;
    public float gravity = -9.8f;
    public float groundDistance = 0.4f;
    public float jumpHeight = 3f;
    public LayerMask groundMask;

    Vector3 velocity;
    bool isGrounded;

    /////////////////////////////////
    ///   Health Bar Variables    ///
    /////////////////////////////////
    public int maxHealth = 100;
    public int currentHealth;
    public HealthBarSlider healthBar;

    /////////////////////////////////
    ///     Weapon Variables      ///
    /////////////////////////////////
    ///
    /////////////////////////////////
    ///       Interactable        ///
    /////////////////////////////////
    //public Interactable focus;

    /////////////////////////////////
    ///          Points           ///
    /////////////////////////////////
    public int currentPoints;
    public int startPoints = 500;


    // Start is called before the first frame update
    void Start()
    {
        /////////////////////////////////
        ///     Health Bar Logic      ///
        /////////////////////////////////
        currentHealth = maxHealth;
        healthBar.SetMaxHealth(maxHealth);
        currentPoints = startPoints;
    }

    // Update is called once per frame
    void Update()
    {
        /////////////////////////////////
        ///  Movement + Camera Logic  ///
        /////////////////////////////////
        isGrounded = Physics.CheckSphere(groundCheck.position, groundDistance, groundMask);

        if (isGrounded && velocity.y < 0)
        {
            velocity.y = -2f;
        }


        float x = Input.GetAxis("Horizontal");
        float z = Input.GetAxis("Vertical");


        
        Vector3 move = transform.right * x + transform.forward * z;
        move.Normalize();                                               //NEW ADDED TO remove strafe speed
        controller.Move(move * speed * Time.deltaTime);

        if (Input.GetButtonDown("Jump") && isGrounded)
        {
            velocity.y = Mathf.Sqrt(jumpHeight * -2f * gravity);
        }

        velocity.y += gravity * Time.deltaTime;

        controller.Move(velocity * Time.deltaTime);

        /////////////////////////////////
        ///       Damage Logic        ///
        /////////////////////////////////
        //test damager
        /*
        if (Input.GetKeyDown(KeyCode.Space))
        {
            TakeDamage(10);
        }
        */

        /////////////////////////////////
        ///       Interactable        ///
        /////////////////////////////////
        /*
        RaycastHit _hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out _hit, range, mask))
            if (Input.GetKeyDown("E"))
        {
            Interactable interactable = _hit.collider.GetComponent<Interactable>();
            if(interactable != null)
            {
                SetFocus(interactable);
            }
        }
        */
    }

    /////////////////////////////////
    ///       Interactable        ///
    /////////////////////////////////
    /*
    void SetFocus (Interactable newFocus)
    {
        focus = newFocus;
    }
    */

    /////////////////////////////////
    ///          Points           ///
    /////////////////////////////////
    public void UpdatePoints(int newPoints)
    {
        currentPoints += newPoints;
    }

    /////////////////////////////////
    ///   Damage Update Function  ///
    /////////////////////////////////
    public void TakeDamage(int damage)
    {
        currentHealth -= damage;
        healthBar.SetHealth(currentHealth);
    }
}
